/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsmereka <gsmereka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 17:38:31 by gsmereka          #+#    #+#             */
/*   Updated: 2023/05/14 18:51:26 by gsmereka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/philo.h"

static void	destroy_forks(t_data *data);
static void	free_generic_array(void **array, int limit);
static void	destroy_philosophers(t_data *data);
static void	destroy_observer(t_data *data);

void	free_data(t_data *data)
{
	destroy_forks(data);
	destroy_philosophers(data);
	destroy_observer(data);
	if (data->philo_threads)
		free_generic_array((void **)data->philo_threads,
			data->config->number_of_philosophers);
	if (data->observer_thread)
		free(data->observer_thread);
	if (data->need_stop_mutex)
	{
		pthread_mutex_destroy(data->need_stop_mutex);
		free(data->need_stop_mutex);
	}
	if (data->print_mutex)
	{
		pthread_mutex_destroy(data->print_mutex);
		free(data->print_mutex);
	}
	if (data->philo_status)
		free_generic_array((void **)data->philo_status,
			data->config->number_of_philosophers);
	if (data->config)
		free(data->config);
}

static void	destroy_philosophers(t_data *data)
{
	int	i;

	i = 0;
	if (data->philosophers)
	{
		while (data->philosophers[i])
		{
			free(data->philosophers[i]->fork_order);
			if (data->philosophers[i]->status)
			{
				pthread_mutex_destroy
					(data->philosophers[i]->status->status_mutex);
				free(data->philosophers[i]->status->status_mutex);
			}
			free(data->philosophers[i]);
			i++;
		}
		free(data->philosophers);
	}
}

static void	destroy_observer(t_data *data)
{
	if (data->observer)
	{
		if (data->observer->missing_meals)
			free(data->observer->missing_meals);
		free(data->observer);
	}
}

static void	destroy_forks(t_data *data)
{
	int	i;

	i = 0;
	if (data->forks)
	{
		while (data->forks[i])
		{
			if (data->forks[i]->mutex)
			{
				pthread_mutex_destroy(data->forks[i]->mutex);
				free(data->forks[i]->mutex);
			}
			free(data->forks[i]);
			i++;
		}
		free(data->forks);
	}
}

static void	free_generic_array(void **array, int limit)
{
	int	i;

	i = 0;
	while (i < limit)
	{
		free(array[i]);
		i++;
	}
	free(array);
}
