/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_routine.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsmereka <gsmereka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/06 18:36:04 by gsmereka          #+#    #+#             */
/*   Updated: 2023/05/13 20:03:45 by gsmereka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/philo.h"

static int	wait_forks(int forks, t_philosopher *philo);

void	*philosopher_routine(void *philosopher)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)philosopher;
	while (philo->eat_limit)
	{
		if (!wait_forks(0, philo))
			return (NULL);
		if (!philo_eat(philo))
			return (NULL);
		philo_sleep_and_think(philo);
		if (philo_need_stop(philo))
			return (NULL);
	}
	return (NULL);
}

static int	wait_forks(int forks, t_philosopher *philo)
{
	int	index;

	index = 0;
	if (philo->id % 2 == 1)
		usleep(10);
	while (forks < 2)
	{
		pthread_mutex_lock(philo->fork_order[index]->mutex);
		if (philo->fork_order[index]->available)
		{
			philo->fork_order[index]->available = FALSE;
			printf("%d %d has taken a fork\n",
				get_time_now() - philo->start_time, philo->id);
			forks++;
		}
		pthread_mutex_unlock(philo->fork_order[index]->mutex);
		if (!index)
			index++;
		else
			index--;
		if (philo_need_stop(philo))
			return (0);
	}
	philo->timer = get_time_now() - philo->start_time;
	return (1);
}

int	philo_need_stop(t_philosopher *philo)
{
	pthread_mutex_lock(*philo->need_stop_mutex);
	if (*philo->need_stop)
	{
		pthread_mutex_unlock(*philo->need_stop_mutex);
		return (1);
	}
	pthread_mutex_unlock(*philo->need_stop_mutex);
	return (0);
}
