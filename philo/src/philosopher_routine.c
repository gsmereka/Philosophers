/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_routine.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsmereka <gsmereka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/06 18:36:04 by gsmereka          #+#    #+#             */
/*   Updated: 2023/05/14 18:44:12 by gsmereka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/philo.h"

static int	wait_forks(int forks, t_philosopher *philo);
static int	is_alone(t_philosopher *philo);
static void	get_forks(t_philosopher *philo);

void	*philosopher_routine(void *philosopher)
{
	t_philosopher	*philo;
	int				wait_time;
	int				fix_time;
	int				delay_timer;

	wait_time = 1000;
	philo = (t_philosopher *)philosopher;
	while (philo->eat_limit)
	{
		fix_time = get_time_now();
		if (!wait_forks(0, philo))
			return (NULL);
		if (!philo_eat(philo))
			return (NULL);
		philo_sleep_and_think(philo);
		if (philo_need_stop(philo))
			return (NULL);
		delay_timer = get_time_now() - fix_time;
		if (wait_time - delay_timer > 0)
			delay_timer = 0;
		usleep(wait_time - delay_timer);
	}
	return (NULL);
}

static int	wait_forks(int forks, t_philosopher *philo)
{
	if (is_alone(philo))
		return (0);
	while (!forks)
	{
		pthread_mutex_lock(philo->fork_order[0]->mutex);
		pthread_mutex_lock(philo->fork_order[1]->mutex);
		if (philo->fork_order[0]->available && philo->fork_order[1]->available)
		{
			get_forks(philo);
			philo->timer = get_time_now() - philo->start_time;
			forks++;
		}
		pthread_mutex_unlock(philo->fork_order[0]->mutex);
		pthread_mutex_unlock(philo->fork_order[1]->mutex);
		if (philo_need_stop(philo))
			return (0);
	}
	return (1);
}

int	philo_need_stop(t_philosopher *philo)
{
	pthread_mutex_lock(*philo->status->need_stop_mutex);
	if (*philo->status->need_stop)
	{
		pthread_mutex_unlock(*philo->status->need_stop_mutex);
		return (1);
	}
	pthread_mutex_unlock(*philo->status->need_stop_mutex);
	return (0);
}

static void	get_forks(t_philosopher *philo)
{
	philo->timer = get_time_now() - philo->start_time;
	philo->fork_order[0]->available = FALSE;
	philo->fork_order[1]->available = FALSE;
	pthread_mutex_lock(*philo->print_mutex);
	printf("%d %d has taken a fork\n",
		philo->timer, philo->id);
	printf("%d %d has taken a fork\n",
		philo->timer, philo->id);
	pthread_mutex_unlock(*philo->print_mutex);
}

static int	is_alone(t_philosopher *philo)
{
	if (philo->is_alone)
	{
		while (TRUE)
		{
			pthread_mutex_lock(philo->fork_order[1]->mutex);
			if (philo->fork_order[1]->available)
			{
				philo->timer = get_time_now() - philo->start_time;
				pthread_mutex_lock(*philo->print_mutex);
				philo->fork_order[1]->available = FALSE;
				printf("%d %d has taken a fork\n",
					philo->timer, philo->id);
				pthread_mutex_unlock(*philo->print_mutex);
			}
			pthread_mutex_unlock(philo->fork_order[1]->mutex);
			if (philo_need_stop(philo))
				return (TRUE);
		}
	}
	return (FALSE);
}
